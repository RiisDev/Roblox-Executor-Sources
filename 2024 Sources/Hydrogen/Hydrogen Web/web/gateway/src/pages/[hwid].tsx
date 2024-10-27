import HCaptcha from "@hcaptcha/react-hcaptcha";
import { StateObject } from "@hydrogen-web/interfaces";
import {
  ActionIcon,
  Anchor,
  Button,
  Stack,
  Text,
  TextInput,
  Title,
  Tooltip,
} from "@mantine/core";
import { useForm } from "@mantine/form";
import { useClipboard, useMediaQuery } from "@mantine/hooks";
import { showNotification } from "@mantine/notifications";
import { IconCheck, IconCopy } from "@tabler/icons";
import { InfoDialog } from "components/Dialogs/InfoDialog";
import { Loader } from "components/Loader";
import { useApi } from "hooks/useApi";
import { NextPage } from "next";
import { useRouter } from "next/router";
import { useEffect, useRef, useState } from "react";
import { relativeTimeUtil } from "utils/relativeTime";

// &#8212;

const Page: NextPage = () => {
  const { fetch } = useApi();
  const router = useRouter();
  const captchaRef = useRef<any>(null);
  const clipboard = useClipboard({ timeout: 3000 });
  const isMobile = useMediaQuery("(max-width: 425px)");
  const [state, setState] = useState<StateObject | false>();
  const [isLoaded, setLoaded] = useState(false);
  const [isSubmitting, setSubmitting] = useState(false);

  const form = useForm({
    initialValues: {
      hwid: "",
      token: "",
      platform: "",
    },
  });

  useEffect(() => {
    if (!router.isReady || !router.query) return;

    const hwid = router.query.hwid as string;
    const fmt = hwid.replace(/\s/g, "");

    if (!fmt || !fmt.length) {
      router.push(`/?error=HWID_NOT_PROVIDED&token=${hwid}_${fmt}`);
      return;
    }

    fetch<StateObject>({
      method: "GET",
      url: `/gateway/${fmt}`,
    })
      .then((response) => {
        setState(response.data);
      })
      .catch(() => {
        setState(false);
      });

    form.setFieldValue("hwid", fmt);
    form.setFieldValue("platform", router.query.platform as string);
    setLoaded(true);
  }, [router.query, router.isReady]);

  return (
    <>
      {state === undefined ? (
        <Loader size="lg" />
      ) : (
        <Stack
          p="xl"
          align="center"
          justify="center"
          sx={{
            flexGrow: 1,
          }}
        >
          <Stack>
            <Title
              weight={500}
              sx={{
                maxInlineSize: 400,
              }}
            >
              {!state
                ? "Welcome to the Hydrogen Gateway"
                : state.key
                ? `We appreciate your support!`
                : `Hydrogen Gateway — Checkpoint ${state.stage}`}
            </Title>
            <Text
              color="dimmed"
              sx={{
                maxInlineSize: 950,
              }}
            >
              {isMobile ? null : (
                <>
                  In order to keep Hydrogen free for everyone to use, we have
                  adopted an ad-based key system. You can avoid this system
                  through a one-time lifetime purchase of{" "}
                  <Anchor component="span" color="blue.2">
                    Hydrogen AdLess
                  </Anchor>
                  .
                  <br />
                  <br />
                </>
              )}
              {state && state.key ? (
                <>
                  Thank you for supporting Hydrogen, your authentication key can
                  be found below. Your key will automatically expire{" "}
                  <Tooltip
                    withArrow
                    label={new Date(state.key.expiresAt).toLocaleString(
                      "en-US",
                      {
                        dateStyle: "long",
                        timeStyle: "long",
                      }
                    )}
                  >
                    <Text
                      weight="bold"
                      component="span"
                      sx={{
                        cursor: "pointer",
                        userSelect: "none",
                      }}
                    >
                      {relativeTimeUtil(new Date(state.key.expiresAt))}
                    </Text>
                  </Tooltip>
                  .
                </>
              ) : (
                <>
                  You will have to go through{" "}
                  {!state ? "three" : `${3 - state.stage + 1} more`} Linkvertise
                  link{state && state.stage === 3 ? "" : "s"} to get your key.
                  Each key will last 24 hours before expiring.
                </>
              )}
            </Text>

            <InfoDialog />

            {state && state.key ? null : (
              <HCaptcha
                sitekey={process.env.NEXT_PUBLIC_HCAPTCHA_SITE_KEY!}
                onVerify={(token) => form.setFieldValue("token", token)}
                theme="dark"
                ref={captchaRef}
              />
            )}

            <form
              onSubmit={form.onSubmit(async (values) => {
                setSubmitting(true);

                if (!values.token) {
                  showNotification({
                    title: "Something went wrong",
                    message:
                      "Please complete the captcha challenge to continue.",
                    color: "red",
                  });
                  return setSubmitting(false);
                }

                try {
                  const response = await fetch<StateObject>({
                    method: "POST",
                    url: `/gateway/${values.hwid}`,
                    data: {
                      token: values.token,
                      platform: values.platform || undefined,
                    },
                  });

                  window.open(response.data.url, "_self");
                } catch (error: any) {
                  showNotification({
                    title: "Something went wrong",
                    message: error.data.error.message,
                    color: "red",
                  });

                  form.reset();
                  form.setFieldValue("hwid", values.hwid);
                  captchaRef.current.resetCaptcha();
                }

                setSubmitting(false);
              })}
            >
              {state && state.key ? (
                <TextInput
                  disabled
                  size="md"
                  radius="xs"
                  value={state.key.value}
                  rightSection={
                    <Tooltip
                      label={clipboard.copied ? "Copied" : "Copy"}
                      withArrow
                      position="right"
                    >
                      <ActionIcon
                        color={clipboard.copied ? "teal" : "gray"}
                        onClick={async () => {
                          clipboard.copy(state.key!.value);
                          showNotification({
                            autoClose: 15000,
                            title: "Warning",
                            message:
                              "If you have any trouble verifying, please try to copy your key directly through the text box.",
                            color: "yellow",
                          });
                        }}
                      >
                        {clipboard.copied ? (
                          <IconCheck size={16} />
                        ) : (
                          <IconCopy size={16} />
                        )}
                      </ActionIcon>
                    </Tooltip>
                  }
                />
              ) : null}

              {state && state.key ? null : (
                <Button
                  size="md"
                  radius="xs"
                  type="submit"
                  variant="gradient"
                  disabled={!isLoaded}
                  loading={isSubmitting}
                  gradient={
                    {
                      deg: 20,
                      to: "#76a4cf",
                    } as any
                  }
                  sx={{
                    width: 300,
                  }}
                >
                  {!state || state.stage >= 3
                    ? "Get Key"
                    : `Continue to Checkpoint ${state.stage + 1}`}
                </Button>
              )}
            </form>
          </Stack>
        </Stack>
      )}
    </>
  );
};

export default Page;
