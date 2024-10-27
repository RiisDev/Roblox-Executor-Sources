const units: { [unit: string]: number } = {
  year: 24 * 60 * 60 * 1000 * 365,
  month: (24 * 60 * 60 * 1000 * 365) / 12,
  day: 24 * 60 * 60 * 1000,
  hour: 60 * 60 * 1000,
  minute: 60 * 1000,
  second: 1000,
};

const rtf = new Intl.RelativeTimeFormat("en", { numeric: "auto" });

export const relativeTimeUtil = (date: Date) => {
  const elapsed = date.getTime() - Date.now();

  for (const unit in units) {
    if (Math.abs(elapsed) > units[unit] || unit == "second") {
      return rtf.format(Math.round(elapsed / units[unit]), unit as any);
    }
  }
};
